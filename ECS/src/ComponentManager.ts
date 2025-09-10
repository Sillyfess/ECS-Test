import { Component, ComponentPool } from './Component';
import { Entity } from './Entity';

class SparseSet<T extends Component> implements ComponentPool<T> {
    private dense: (T | undefined)[] = [];
    private sparse: Map<number, number> = new Map();

    public get(entityId: number): T | undefined {
        const index = this.sparse.get(entityId);
        return index !== undefined ? this.dense[index] : undefined;
    }

    public set(entityId: number, component: T): void {
        if (this.sparse.has(entityId)) {
            const index = this.sparse.get(entityId)!;
            this.dense[index] = component;
        } else {
            const index = this.dense.length;
            this.dense.push(component);
            this.sparse.set(entityId, index);
        }
    }

    public remove(entityId: number): void {
        const index = this.sparse.get(entityId);
        if (index !== undefined) {
            const lastIndex = this.dense.length - 1;
            
            if (index !== lastIndex) {
                this.dense[index] = this.dense[lastIndex];
                
                for (const [eid, idx] of this.sparse.entries()) {
                    if (idx === lastIndex) {
                        this.sparse.set(eid, index);
                        break;
                    }
                }
            }
            
            this.dense.pop();
            this.sparse.delete(entityId);
        }
    }

    public clear(): void {
        this.dense = [];
        this.sparse.clear();
    }

    public getAll(): T[] {
        return this.dense.filter(c => c !== undefined) as T[];
    }
}

export class ComponentManager {
    private componentPools: Map<number, SparseSet<Component>> = new Map();
    private componentTypes: Map<number, new (...args: any[]) => Component> = new Map();

    public registerComponent<T extends Component>(componentClass: new (...args: any[]) => T): void {
        const typeId = Component.getTypeId(componentClass);
        if (!this.componentPools.has(typeId)) {
            this.componentPools.set(typeId, new SparseSet<T>());
            this.componentTypes.set(typeId, componentClass);
        }
    }

    public addComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T,
        component: T
    ): void {
        const typeId = Component.getTypeId(componentClass);
        
        if (!this.componentPools.has(typeId)) {
            this.registerComponent(componentClass);
        }

        const pool = this.componentPools.get(typeId)!;
        pool.set(entity.id, component);
        entity.addComponentBit(typeId);
    }

    public getComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T
    ): T | undefined {
        const typeId = Component.getTypeId(componentClass);
        const pool = this.componentPools.get(typeId);
        
        if (!pool || !entity.hasComponentBit(typeId)) {
            return undefined;
        }

        return pool.get(entity.id) as T | undefined;
    }

    public removeComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T
    ): void {
        const typeId = Component.getTypeId(componentClass);
        const pool = this.componentPools.get(typeId);
        
        if (pool && entity.hasComponentBit(typeId)) {
            pool.remove(entity.id);
            entity.removeComponentBit(typeId);
        }
    }

    public hasComponent<T extends Component>(
        entity: Entity,
        componentClass: new (...args: any[]) => T
    ): boolean {
        const typeId = Component.getTypeId(componentClass);
        return entity.hasComponentBit(typeId);
    }

    public getAllComponents<T extends Component>(
        componentClass: new (...args: any[]) => T
    ): T[] {
        const typeId = Component.getTypeId(componentClass);
        const pool = this.componentPools.get(typeId) as SparseSet<T> | undefined;
        return pool ? pool.getAll() : [];
    }

    public clearEntityComponents(entity: Entity): void {
        for (const [typeId, pool] of this.componentPools) {
            if (entity.hasComponentBit(typeId)) {
                pool.remove(entity.id);
            }
        }
    }

    public clear(): void {
        for (const pool of this.componentPools.values()) {
            pool.clear();
        }
    }
}