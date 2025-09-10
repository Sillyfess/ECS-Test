import { System } from '../System';
export declare class BoundsSystem extends System {
    private bounds;
    constructor();
    setBounds(minX: number, maxX: number, minY: number, maxY: number, minZ: number, maxZ: number): void;
    update(deltaTime: number): void;
}
//# sourceMappingURL=BoundsSystem.d.ts.map