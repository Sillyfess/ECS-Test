import { Component } from '../Component';
export declare class Health extends Component {
    current: number;
    max: number;
    constructor(max?: number, current?: number);
    takeDamage(amount: number): void;
    heal(amount: number): void;
    isDead(): boolean;
    getPercentage(): number;
    clone(): Health;
}
//# sourceMappingURL=Health.d.ts.map